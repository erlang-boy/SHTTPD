CFLAGS = -O2
OBJSDIR:= .objs
OBJS   = shttpd.o shttpd_params.o 
TARGET = shttpd 
$(TARGET):$(OBJSDIR) $(OBJS)
	gcc  -o $@ $(OBJSDIR)/*.o -g 
$(OBJS):%.o:%.c
	gcc  -c $< $(CFLAGS) -o $@ -g
	mv $@ $(OBJSDIR)/
$(OBJSDIR):
	mkdir -p ./$@
clean:
	-$(RM) $(TARGET) 
	-$(RM) $(OBJSDIR)/*.o
