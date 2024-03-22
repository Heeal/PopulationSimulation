#define DEBUG

#ifdef DEBUG
    #define DEBUG_INFO(...) do{ fprintf( stdout, "[INFO] "__VA_ARGS__ ); } while( 0 )
    #define DEBUG_WARN(...) do{ fprintf( stdout, "[WARN] "__VA_ARGS__ ); } while( 0 )
    #define DEBUG_ERROR(...) do{ fprintf( stderr, "[ERROR] "__VA_ARGS__ ); } while( 0 )
    #else
    def DEBUG
    #define DEBUG_INFO(...) 
    #define DEBUG_WARN(...) 
    #define DEBUG_ERROR(...) 
#endif

#define FIRSTNAME_MAX_SIZE 50