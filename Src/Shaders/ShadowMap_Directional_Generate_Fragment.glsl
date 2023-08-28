#version 330 core

void main() {
    // gl_FragDepth = gl_FragCoord.z;
    // 我们可以取消这行的注释来显式设置深度，但是我们让GPU自己默认地去做这件事却会更有效率，因为底层无论如何都会默认去设置深度缓冲。
}