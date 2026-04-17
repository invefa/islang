#ifndef ISC_ARTIFACT_H
#define ISC_ARTIFACT_H

#include "isl_instream.h"
#include "isl_value.h"

typedef struct ist_artifact {
    ist_value*       consts;
    ist_instruction* instream;
} ist_artifact;

#endif // ISC_ARTIFACT_H