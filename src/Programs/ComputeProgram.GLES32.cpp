#include <Veritas/GPU/Programs/ComputeProgram.h>

#include <Veritas/GPU/Definitions.h>

using namespace Veritas;
using namespace GPU;

using namespace Programs;

using namespace Math;

#include <iostream>
using std::string;

uint32 createComputeShaderProgram(const string& icode) {
    string code = string("#version 320 es\n")
                + icode;

    const char *source = code.c_str();
    return glCreateShaderProgramv(GL_COMPUTE_SHADER, 1, &source);
}

ComputeProgram::ComputeProgram(const string &icode) : Program(createComputeShaderProgram(icode)) {
    int32 linked;
    glGetProgramiv(getID(), GL_LINK_STATUS, &linked);
    if (!linked) {
        int length;
        glGetProgramiv(getID(), GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        glGetProgramInfoLog(getID(), length, &length, log);
        std::cerr << log << std::endl;
        delete[] log;
    }

    glGetProgramiv(getID(), GL_COMPUTE_WORK_GROUP_SIZE, (int32*) &groupSize);
}

ComputeProgram::~ComputeProgram() {
    glDeleteProgram(getID());
}

void ComputeProgram::compute(const uint32 work, Bindings bps) {
    compute(uvec3(work, 1, 1), bps);
}

void ComputeProgram::compute(const uvec2 &work, Bindings bps) {
    compute(uvec3(work.x, work.y, 1), bps);
}

void ComputeProgram::compute(const uvec3& work, Bindings bps) {
    for (auto bp : bps) bp.bind();
    glUseProgram(getID());

    glDispatchCompute(work.x / groupSize.x, work.y / groupSize.y, work.z / groupSize.z);

    for (auto bp : bps) bp.unbind();
    glUseProgram(0);
}

const uvec3 &ComputeProgram::getGroupSize() const { return groupSize; }
