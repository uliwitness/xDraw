Boolean	DoLineOpcode( char* string, short *x, unsigned long flags );
Boolean	DoRectOpcode( char* string, short *x, unsigned long flags );
Boolean	DoOvalOpcode( char* string, short *x, unsigned long flags );
Boolean	DoRoundRectOpcode( char* string, short *x, unsigned long flags );
Boolean	DoArcOpcode( char* string, short *x, unsigned long flags );
Boolean	DoStringOpcode( char* string, short *x, unsigned long flags );
Boolean	DoTextOpcode( char* string, short *x, unsigned long flags );
Boolean	DoPictOpcode( char* string, short *x, unsigned long flags );
Boolean	DoPictFileOpcode( char* string, short *x, unsigned long flags );
Boolean	DoIconOpcode( char* string, short *x, unsigned long flags );
Boolean	DoPolyOpcode( char* string, short *x, unsigned long flags );
Boolean	DoPenSizeOpcode( char* string, short *x, unsigned long flags );
Boolean	DoPenModeOpcode( char* string, short *x, unsigned long flags );
Boolean	DoFieldOpcode( char* string, short *x, unsigned long flags );
Boolean	DoButtonOpcode( char* string, short *x, unsigned long flags );
Boolean	DoFieldTextOpcode( char* string, short *x, unsigned long flags );

void	CenterRectInRect( Rect* destRect, Rect* boundsRect );
void	FitRectInRect( Rect* destRect, Rect* boundsRect );
void	mixColor( RGBColor *light, RGBColor *dark, short shade, 
					RGBColor *result );