if ARGV[0].nil?
        puts "Specify openwrt/wiveng"
        exit(0)
end

begin
        if ARGV[0].strip == "wiveng"
           system("ruby mk.rb #{ARGV[0]} wng_server.c")
           %x[scp wng_server Admin@192.168.1.1:/tmp]
        elsif  ARGV[0].strip == "openwrt"
           system("ruby mk.rb #{ARGV[0]} owrt_server.c")
           %x[scp owrt_server root@192.168.1.1:/tmp]
        end
rescue Exception => e
        puts e.message
        exit(1)
end

