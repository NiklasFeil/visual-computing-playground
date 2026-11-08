inline void setUniformImpl(const int location, const float v) const {
    glUniform1f(location, v);
}
inline void setUniformImpl(const int location, const glm::vec2 v) const {
    glUniform2f(location, v.x, v.y);
}
inline void setUniformImpl(const int location, const glm::vec3 v) const {
    glUniform3f(location, v.x, v.y, v.z);
}
inline void setUniformImpl(const int location, const glm::vec4 v) const {
    glUniform4f(location, v.x, v.y, v.z, v.w);
}

inline void setUniformImpl(const int location, const int v) const {
    glUniform1i(location, v);
}
inline void setUniformImpl(const int location, const glm::ivec2 v) const {
    glUniform2i(location, v.x, v.y);
}
inline void setUniformImpl(const int location, const glm::ivec3 v) const {
    glUniform3i(location, v.x, v.y, v.z);
}
inline void setUniformImpl(const int location, const glm::ivec4 v) const {
    glUniform4i(location, v.x, v.y, v.z, v.w);
}

inline void setUniformImpl(const int location, const unsigned int v) const {
    glUniform1ui(location, v);
}
inline void setUniformImpl(const int location, const glm::uvec2 v) const {
    glUniform2ui(location, v.x, v.y);
}
inline void setUniformImpl(const int location, const glm::uvec3 v) const {
    glUniform3ui(location, v.x, v.y, v.z);
}
inline void setUniformImpl(const int location, const glm::uvec4 v) const {
    glUniform4ui(location, v.x, v.y, v.z, v.w);
}

inline void setUniformImpl(const int location, const glm::mat4 v) const {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
}