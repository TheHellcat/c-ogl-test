#include <string>

unsigned int CreateShader( std::string &vertexShader, std::string &fragmentShader );
unsigned int CompileShader( std::string &source, unsigned int type );
void GlClearError();
bool GlCheckError(const char* func, const char* file, int line);
int generateNormalData( float* vertices, int vertex_stride, unsigned int* indices, int count, float* &normal_data );
