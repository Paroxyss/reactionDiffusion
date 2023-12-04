#define WIDTH 1500
#define HEIGHT 800

#define RDBullesDeSavon

#ifdef RDVagues
#define diffusionA 1 // 1.0
#define diffusionB 0.5 // 0.5
#define feedRate 0.0030 // 0.055
#define killRate 0.039 // 0.062
#define dt 0.6 
#endif

#ifdef RDBullesDeSavon
#define diffusionA 1.2 // 1.0
#define diffusionB 0.8 // 0.5
#define feedRate 0.055 // 0.055
#define killRate 0.044 // 0.062
#define dt 1
#endif

#ifdef RDprocesseur
#define diffusionA 0.19 // 1.0
#define diffusionB 0.1 // 0.5
#define feedRate 0.055 // 0.055
#define killRate 0.062 // 0.062
#define dt 1
#endif

#ifdef RDLent
#define diffusionA 0.481 // 1.0
#define diffusionB 0.05 // 0.5
#define feedRate 0.055 // 0.055
#define killRate 0.079 // 0.062
#define dt 1
#endif

#ifdef RDclassic
#define diffusionA 1.0 // 1.0
#define diffusionB 0.5 // 0.5
#define feedRate 0.055 // 0.055
#define killRate 0.062 // 0.062
#define dt 1
#endif
