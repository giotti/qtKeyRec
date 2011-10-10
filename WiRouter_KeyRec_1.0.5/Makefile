AGPF_CONF   = config/agpf_config.lst
SOURCE      = src/wirouterkeyrec.c src/agpf.c src/sha256.c src/md5.c src/hashword.c src/pirelli.c src/telsey.c src/socket.c
EXEC        = wirouterkeyrec
LIBS        = 
CFLAGS      = -O2 -Wall -Wextra
DEBUG       = -g

wirouterkeyrec:
	$(CC) $(LIBS) $(SOURCE) $(CFLAGS) -o $(EXEC)

debug:
	$(CC) $(LIBS) $(SOURCE) $(CFLAGS) $(DEBUG) -o $(EXEC)

install:
	mkdir /etc/wirouterkeyrec
	install $(EXEC) $(DESTDIR)/usr/local/bin
	install $(AGPF_CONF) $(DESTDIR)/etc/wirouterkeyrec
	chmod 755 $(DESTDIR)/usr/local/bin/$(EXEC)

uninstall:
	rm -rf /etc/wirouterkeyrec
	rm -f $(DESTDIR)/usr/local/bin/$(EXEC)

clean:
	@rm -f $(EXEC)

.PHONY: wirouterkeyrec debug install uninstall clean
