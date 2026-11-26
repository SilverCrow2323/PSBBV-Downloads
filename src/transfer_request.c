#include "psbbv/downloads/transfer_request.h"
#include <ctype.h>
#include <stddef.h>
#include <string.h>

static int terminated(const char *value, size_t capacity) {
    return value != NULL && memchr(value, '\0', capacity) != NULL;
}
static size_t bounded_length(const char *value, size_t capacity) {
    const char *end = (const char *)memchr(value, '\0', capacity);
    return end ? (size_t)(end - value) : capacity;
}
static int has_dot_segment(const char *path) {
    const char *cursor = path;
    while (*cursor) {
        const char *end = strchr(cursor, '/');
        size_t length = end ? (size_t)(end - cursor) : strlen(cursor);
        if ((length == 1 && cursor[0] == '.') ||
            (length == 2 && cursor[0] == '.' && cursor[1] == '.')) return 1;
        if (!end) break;
        cursor = end + 1;
    }
    return 0;
}
static int allowed_mount(const char *root) {
    static const char *mounts[] = {"ux0:/", "ur0:/", "uma0:/", "imc0:/", "xmc0:/"};
    size_t i;
    for (i = 0; i < sizeof(mounts) / sizeof(mounts[0]); ++i)
        if (strncmp(root, mounts[i], strlen(mounts[i])) == 0) return 1;
    return 0;
}
static int safe_filename(const char *name) {
    size_t i, length = strlen(name);
    if (!length || length >= PSBBV_TRANSFER_NAME_CAP ||
        strcmp(name, ".") == 0 || strcmp(name, "..") == 0 ||
        name[length - 1] == '.' || name[length - 1] == ' ') return 0;
    for (i = 0; i < length; ++i) {
        unsigned char c = (unsigned char)name[i];
        if (!(isalnum(c) || c == '.' || c == '_' || c == '-')) return 0;
    }
    return 1;
}
static int valid_sha256(const char *hash) {
    size_t i;
    if (strlen(hash) != 64) return 0;
    for (i = 0; i < 64; ++i)
        if (!((hash[i] >= '0' && hash[i] <= '9') ||
              (hash[i] >= 'a' && hash[i] <= 'f') ||
              (hash[i] >= 'A' && hash[i] <= 'F'))) return 0;
    return 1;
}
static psbbv_transfer_request_result validate_url(const char *url) {
    const char *authority, *end;
    size_t i, length = strlen(url), authority_length;
    if (length < 9 || strncmp(url, "https://", 8) != 0) return PSBBV_REQUEST_URL_SCHEME;
    authority = url + 8;
    end = strpbrk(authority, "/?#");
    if (!end) end = url + length;
    authority_length = (size_t)(end - authority);
    if (!authority_length) return PSBBV_REQUEST_URL_HOST;
    if (memchr(authority, '@', authority_length)) return PSBBV_REQUEST_URL_CREDENTIALS;
    for (i = 0; i < authority_length; ++i)
        if (isspace((unsigned char)authority[i]) || authority[i] == '\\')
            return PSBBV_REQUEST_URL_HOST;
    if (strchr(end, '#')) return PSBBV_REQUEST_URL_FRAGMENT;
    return PSBBV_REQUEST_VALID;
}
psbbv_transfer_request_result psbbv_transfer_validate_request(
    const psbbv_transfer_request *request, const char *allowed_root
) {
    size_t root_length, destination_length, filename_length;
    psbbv_transfer_request_result url_result;
    if (!request || !allowed_root ||
        !terminated(request->url, sizeof(request->url)) ||
        !terminated(request->filename, sizeof(request->filename)) ||
        !terminated(request->destination, sizeof(request->destination)) ||
        !terminated(request->sha256, sizeof(request->sha256)) ||
        !memchr(allowed_root, '\0', PSBBV_TRANSFER_PATH_CAP))
        return PSBBV_REQUEST_INVALID_ARGUMENT;
    if (!request->url[0] || !request->filename[0] || !request->destination[0])
        return PSBBV_REQUEST_INVALID_ARGUMENT;
    url_result = validate_url(request->url);
    if (url_result != PSBBV_REQUEST_VALID) return url_result;
    if (!safe_filename(request->filename)) return PSBBV_REQUEST_FILENAME;
    root_length = strlen(allowed_root);
    destination_length = strlen(request->destination);
    filename_length = strlen(request->filename);
    if (root_length < 6 || root_length >= PSBBV_TRANSFER_PATH_CAP ||
        !allowed_mount(allowed_root) || allowed_root[root_length - 1] == '/' ||
        has_dot_segment(allowed_root) || strstr(allowed_root, "//"))
        return PSBBV_REQUEST_DESTINATION;
    if (destination_length != root_length + 1 + filename_length ||
        strncmp(request->destination, allowed_root, root_length) != 0 ||
        request->destination[root_length] != '/' ||
        strcmp(request->destination + root_length + 1, request->filename) != 0 ||
        has_dot_segment(request->destination) || strstr(request->destination, "//"))
        return PSBBV_REQUEST_DESTINATION;
    if (!request->max_bytes || request->max_bytes > PSBBV_TRANSFER_HARD_LIMIT ||
        (request->expected_bytes && request->expected_bytes > request->max_bytes))
        return PSBBV_REQUEST_SIZE_POLICY;
    if (request->require_sha256 && !valid_sha256(request->sha256))
        return PSBBV_REQUEST_CHECKSUM;
    if (request->sha256[0] && !valid_sha256(request->sha256))
        return PSBBV_REQUEST_CHECKSUM;
    return PSBBV_REQUEST_VALID;
}
