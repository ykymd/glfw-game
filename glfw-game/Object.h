#pragma once

// 図形データ
class Object
{
    // 頂点配列オブジェクト
    GLuint vao;
    
    // 頂点バッファオブジェクト
    GLuint vbo;
    
public:
    // 頂点属性
    struct Vertex
    {
        GLfloat position[2];
    };
    
    Object(GLint size, GLuint vertexcount, const Vertex *vertex)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }
    
    void bind() const
    {
        glBindVertexArray(vao);
    }
    
    virtual ~Object()
    {
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
private:
    // コピーコンストラクタによるコピー禁止
    Object(const Object &o);
    
    // 代入によるコピー禁止
    Object &operator=(const Object &o);
};
