#include "psbbv/downloads/transfer_job.h"
#include <stdio.h>
#define CHECK(expression) do { if (!(expression)) { fprintf(stderr, "CHECK failed at %s:%d: %s\n", __FILE__, __LINE__, #expression); return 1; } } while (0)
int main(void) {
 psbbv_transfer_job job;
 psbbv_transfer_init(&job,1000);
 CHECK(psbbv_transfer_transition(&job,PSBBV_TRANSFER_DOWNLOADING)==PSBBV_TRANSFER_BAD_TRANSITION);
 CHECK(psbbv_transfer_transition(&job,PSBBV_TRANSFER_CONNECTING)==PSBBV_TRANSFER_OK);
 CHECK(psbbv_transfer_transition(&job,PSBBV_TRANSFER_DOWNLOADING)==PSBBV_TRANSFER_OK);
 CHECK(psbbv_transfer_set_progress(&job,500)==PSBBV_TRANSFER_OK);
 CHECK(psbbv_transfer_set_progress(&job,400)==PSBBV_TRANSFER_INVALID);
 CHECK(psbbv_transfer_set_progress(&job,1001)==PSBBV_TRANSFER_SIZE_EXCEEDED);
 CHECK(psbbv_transfer_transition(&job,PSBBV_TRANSFER_PAUSED)==PSBBV_TRANSFER_OK);
 CHECK(psbbv_transfer_set_progress(&job,600)==PSBBV_TRANSFER_INVALID);
 CHECK(psbbv_transfer_transition(&job,PSBBV_TRANSFER_DOWNLOADING)==PSBBV_TRANSFER_OK);
 CHECK(psbbv_transfer_set_progress(&job,1000)==PSBBV_TRANSFER_OK);
 CHECK(psbbv_transfer_transition(&job,PSBBV_TRANSFER_VERIFYING)==PSBBV_TRANSFER_OK);
 CHECK(psbbv_transfer_transition(&job,PSBBV_TRANSFER_COMPLETED)==PSBBV_TRANSFER_OK);
 CHECK(psbbv_transfer_transition(&job,PSBBV_TRANSFER_FAILED)==PSBBV_TRANSFER_BAD_TRANSITION);
 puts("PSBBV transfer state-machine tests passed"); return 0;
}
