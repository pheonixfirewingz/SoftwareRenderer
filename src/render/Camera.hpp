#pragma once
#include "../math/Vector3.hpp"
#include "../math/Matrix4x4.hpp"
#include "../math/Quaternion.hpp"
class Camera
{
    Vector3 position;
    Vector3 scale;
    SSE3Quaternion rotation;
    public:
    Camera (Vector3 position_,Vector3 scale_,SSE3Quaternion rotation_): position(position_), scale(scale_), rotation(rotation_){}


    Matrix4x4 getProjectionMatrix() {
        Matrix4x4 projection;
        return projection;
    }

    Matrix4x4 getTranslationMatrix() {
        return Matrix4x4().makeTransformMatrix(position,rotation,scale);
    }
};