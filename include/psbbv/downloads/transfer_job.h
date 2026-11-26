#ifndef PSBBV_DOWNLOADS_TRANSFER_JOB_H
#define PSBBV_DOWNLOADS_TRANSFER_JOB_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
 PSBBV_TRANSFER_QUEUED=0, PSBBV_TRANSFER_CONNECTING, PSBBV_TRANSFER_DOWNLOADING,
 PSBBV_TRANSFER_PAUSED, PSBBV_TRANSFER_VERIFYING, PSBBV_TRANSFER_COMPLETED,
 PSBBV_TRANSFER_FAILED, PSBBV_TRANSFER_CANCELLED
} psbbv_transfer_state;
typedef enum { PSBBV_TRANSFER_OK=0, PSBBV_TRANSFER_INVALID, PSBBV_TRANSFER_BAD_TRANSITION, PSBBV_TRANSFER_SIZE_EXCEEDED, PSBBV_TRANSFER_INCOMPLETE } psbbv_transfer_result;
typedef struct { psbbv_transfer_state state; uint64_t expected_bytes; uint64_t received_bytes; } psbbv_transfer_job;
void psbbv_transfer_init(psbbv_transfer_job *job,uint64_t expected_bytes);
psbbv_transfer_result psbbv_transfer_transition(psbbv_transfer_job *job,psbbv_transfer_state next);
psbbv_transfer_result psbbv_transfer_set_progress(psbbv_transfer_job *job,uint64_t received_bytes);
#ifdef __cplusplus
}
#endif
#endif
