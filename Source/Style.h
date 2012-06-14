/* STYLE.H
 * This header file does absolutely nothing.
 * It includes code style information to maintain
 * uniformity and consistency in my naming convention
 * and maintain cleanliness throughout the code base.
 */

/* Style:
 *
 * Class names: CamelCase. If requiring OpenGL, prefix with CGL_,
 * if requiring SDL, prefix CSDL_, otherwise
 * just prefix with C.
 * Struct names: Same rules as class names except no C prefix
 * Enum names: Same rules as struct
 *      Ex: class CGL_Entity; class CSDL_Display; enum GameState; struct GL_Rect
 *
 * Method names: CamelCase
 *      Ex: CGL_Entity::LoadEntity(const char* file_name);
 *
 * Function names: lowercase_underscore
 *      Ex: blit_surface(SDL_Surface* Source, SDL_Surface* destination, const GL_Vertex2f& Pos);
 *
 * Simple variables (int, char, etc): lowercase_underscore
 *      Ex: const char* some_text = NULL;
 *
 * Classes: CamelCase
 *      Ex: CSettings SettingsManager;
 *
 * Structs: Uppercase_Underscore
 *      Ex: SDL_Surface* Main_Screen;
 *
 * STL containers (vector, map, etc): Uppercase_Underscore
 *      Ex: std::vector<int> Player_Scores;
 *
 * STL variables (string, fstream): lowercase_underscore
 *      Ex: void CMap::Load(ifstream& file, string& file_name);
 *
 * Any classes or functions or anything really, should be encompassed
 * in a namespace prefixed with gk, then _sdl if SDL functions, or _gl
 * if OpenGL functions. So in the end there should be three namespaces
 * total: gk, for generic things; gk_gl, for OpenGL classes, functions;
 * and gk_sdl, for SDL classes, functions, etc.
 *      Ex: namespace gk_sdl { class CSDL_Display; }
 *
 *
 * When creating classes, the ordering of stuff should go as follows:
 *      - Ctor / Dtor
 *      - Public methods
 *      - Public get/set methods
 *      - Public virtual methods
 *      - Public pure virtual methods ( = 0)
 *      - Public static methods
 *      - Public variables
 *          - static
 *          - const
 *          - enums / structs / classes (that order)
 *          - STL containers
 *          - STL vars
 *          - simple vars
 *      - Repeat with private
 */
