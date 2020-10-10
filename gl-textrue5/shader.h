
#include <GL/glew.h>
#include <string>

using namespace std;

class Shader
{
public:
    Shader(void);
    ~Shader(void);

    GLuint LoadShader(const char* vertPath, const char* fragPath);
private:
    string ReadFile(const char* path);
};
