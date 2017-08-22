#pragma once
#include <memory>

// 図形データ
#include "Object.h"

// 図形の描画
class Shape
{
    std::shared_ptr<const Object> object;

protected:
    // 描画に使う頂点の数
    GLsizei vertexcount;
    
public:
    Shape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex)
        : object(new Object(size, vertexcount, vertex))
        , vertexcount(vertexcount)
    {
    }
    
    void draw() const
    {
        object->bind();
        execute();
    }
    
    virtual void execute() const
    {
        glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
    }
};
