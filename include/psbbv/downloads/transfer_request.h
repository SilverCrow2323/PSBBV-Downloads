#ifndef PSBBV_DOWNLOADS_TRANSFER_REQUEST_H
#define PSBBV_DOWNLOADS_TRANSFER_REQUEST_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define PSBBV_TRANSFER_URL_CAP 2048
#define PSBBV_TRANSFER_NAME_CAP 128
#define PSBBV_TRANSFER_PATH_CAP 512
#define PSBBV_TRANSFER_SHA256_CAP 65
#define PSBBV_TRANSFER_HARD_LIMIT UINT64_C(4294967295)
typedef struct {
    char url[PSBBV_TRANSFER_URL_CAP];
    char filename[PSBBV_TRANSFER_NAME_CAP];
    char destination[PSBBV_TRANSFER_PATH_CAP];
    uint64_t expected_bytes;
    uint64_t max_bytes;
    char sha256[PSBBV_TRANSFER_SHA256_CAP];
    int require_sha256;
} psbbv_transfer_request;
typedef enum {
    PSBBV_REQUEST_VALID = 0,
    PSBBV_REQUEST_INVALID_ARGUMENT,
    PSBBV_REQUEST_URL_SCHEME,
    PSBBV_REQUEST_URL_HOST,
    PSBBV_REQUEST_URL_CREDENTIALS,
    PSBBV_REQUEST_URL_FRAGMENT,
    PSBBV_REQUEST_FILENAME,
    PSBBV_REQUEST_DESTINATION,
    PSBBV_REQUEST_SIZE_POLICY,
    PSBBV_REQUEST_CHECKSUM
} psbbv_transfer_request_result;
psbbv_transfer_request_result psbbv_transfer_validate_request(
    const psbbv_transfer_request *request,
    const char *allowed_root
);
#ifdef __cplusplus
}
#endif
#endif
