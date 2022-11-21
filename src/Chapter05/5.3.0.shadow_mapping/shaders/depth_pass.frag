#version 330 core

void main() {
    // Since this depth pass does not write to the color buffer. We can leave this empty.
    // To explicitly set the depth of the fragment we can also write:
    // gl_FragDepth = gl_FragCoord.z;
    // But this is what happens automatically if we leave this empty. 
}