LDFLAGS = gdi32.lib user32.lib

calculator_parse_1: 
	cl main.c calculator_parse_1.c basic_stuff.c $(LDFLAGS) /Zi /Od

calculator_parse_2: 
	cl main.c calculator_parse_2.c basic_stuff.c $(LDFLAGS) /Zi /Od

clean:
	del *.ilk
	del *.exe
	del *.obj
	del *.pdb