#include "psbbv/downloads/transfer_request.h"
#include <stdio.h>
#include <string.h>
#define CHECK(expression) do { if (!(expression)) { fprintf(stderr, "CHECK failed at %s:%d: %s\n", __FILE__, __LINE__, #expression); return 1; } } while (0)
static psbbv_transfer_request valid_request(void) {
    psbbv_transfer_request request;
    memset(&request, 0, sizeof(request));
    strcpy(request.url, "https://downloads.example.invalid/app.vpk?channel=stable");
    strcpy(request.filename, "app.vpk");
    strcpy(request.destination, "ux0:/data/PSBBV/Downloads/app.vpk");
    request.expected_bytes = 1024;
    request.max_bytes = 4 * 1024 * 1024;
    memset(request.sha256, 'a', 64);
    request.sha256[64] = '\0';
    request.require_sha256 = 1;
    return request;
}
int main(void) {
    psbbv_transfer_request request = valid_request();
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_VALID);
    strcpy(request.url, "http://downloads.example.invalid/app.vpk");
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_URL_SCHEME);
    request = valid_request(); strcpy(request.url, "https://user:pass@downloads.example.invalid/app.vpk");
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_URL_CREDENTIALS);
    request = valid_request(); strcpy(request.url, "https://downloads.example.invalid/app.vpk#fragment");
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_URL_FRAGMENT);
    request = valid_request(); strcpy(request.filename, "../escape.vpk");
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_FILENAME);
    request = valid_request(); strcpy(request.destination, "ux0:/data/PSBBV/Downloads/../escape.vpk");
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_DESTINATION);
    request = valid_request(); strcpy(request.destination, "ux0:/data/Other/app.vpk");
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_DESTINATION);
    request = valid_request(); request.expected_bytes = request.max_bytes + 1;
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_SIZE_POLICY);
    request = valid_request(); request.require_sha256 = 1; request.sha256[0] = '\0';
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_CHECKSUM);
    request = valid_request(); strcpy(request.sha256, "xyz");
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/Downloads") == PSBBV_REQUEST_CHECKSUM);
    request = valid_request();
    CHECK(psbbv_transfer_validate_request(&request, "ux0:/data/PSBBV/../Downloads") == PSBBV_REQUEST_DESTINATION);
    puts("PSBBV transfer request policy tests passed");
    return 0;
}
