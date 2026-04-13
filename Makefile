LDFLAGS = gdi32.lib user32.lib

main: 
	cl main.c calculator_state.c basic_stuff.c $(LDFLAGS) /Zi /Od

clean:
	del *.ilk
	del *.exe
	del *.obj
	del *.pdb