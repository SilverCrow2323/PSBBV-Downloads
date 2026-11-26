#include "psbbv/downloads/transfer_job.h"
void psbbv_transfer_init(psbbv_transfer_job *job,uint64_t expected_bytes) {
 if(job){job->state=PSBBV_TRANSFER_QUEUED;job->expected_bytes=expected_bytes;job->received_bytes=0;}
}
static int allowed(psbbv_transfer_state from,psbbv_transfer_state to) {
 switch(from) {
 case PSBBV_TRANSFER_QUEUED: return to==PSBBV_TRANSFER_CONNECTING||to==PSBBV_TRANSFER_CANCELLED;
 case PSBBV_TRANSFER_CONNECTING: return to==PSBBV_TRANSFER_DOWNLOADING||to==PSBBV_TRANSFER_FAILED||to==PSBBV_TRANSFER_CANCELLED;
 case PSBBV_TRANSFER_DOWNLOADING: return to==PSBBV_TRANSFER_PAUSED||to==PSBBV_TRANSFER_VERIFYING||to==PSBBV_TRANSFER_FAILED||to==PSBBV_TRANSFER_CANCELLED;
 case PSBBV_TRANSFER_PAUSED: return to==PSBBV_TRANSFER_DOWNLOADING||to==PSBBV_TRANSFER_CANCELLED;
 case PSBBV_TRANSFER_VERIFYING: return to==PSBBV_TRANSFER_COMPLETED||to==PSBBV_TRANSFER_FAILED;
 default: return 0;
 }
}
psbbv_transfer_result psbbv_transfer_transition(psbbv_transfer_job *job,psbbv_transfer_state next) {
 if(!job||next<PSBBV_TRANSFER_QUEUED||next>PSBBV_TRANSFER_CANCELLED) return PSBBV_TRANSFER_INVALID;
 if(!allowed(job->state,next)) return PSBBV_TRANSFER_BAD_TRANSITION;
 if(next==PSBBV_TRANSFER_VERIFYING && job->expected_bytes && job->received_bytes!=job->expected_bytes) return PSBBV_TRANSFER_INCOMPLETE;
 job->state=next; return PSBBV_TRANSFER_OK;
}
psbbv_transfer_result psbbv_transfer_set_progress(psbbv_transfer_job *job,uint64_t bytes) {
 if(!job||job->state!=PSBBV_TRANSFER_DOWNLOADING) return PSBBV_TRANSFER_INVALID;
 if(bytes<job->received_bytes) return PSBBV_TRANSFER_INVALID;
 if(job->expected_bytes&&bytes>job->expected_bytes) return PSBBV_TRANSFER_SIZE_EXCEEDED;
 job->received_bytes=bytes; return PSBBV_TRANSFER_OK;
}
