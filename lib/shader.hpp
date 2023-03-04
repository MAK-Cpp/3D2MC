#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

#include <filesystem>

GLuint LoadShaders(const std::filesystem::path vertex_file_path,
                   const std::filesystem::path fragment_file_path);

#endif
