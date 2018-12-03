
begin
           system("mips-mti-linux-gnu-gcc -static -EL sniffex.c -o sniffex -lpcap")
           %x[scp sniffex Admin@192.168.1.1:/tmp]
rescue Exception => e
        puts e.message
        exit(1)
end

