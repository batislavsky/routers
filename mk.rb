
if ARGV[0] == nil || ARGV[1] == nil
  puts "Please, specify the arguments"
  exit(0)
end

src_dir = "~/Desktop/" + ARGV[1]
executable = "~/Desktop/" + ARGV[1].split('.').first

begin
   unless Dir.exist?("#{ENV['MIPS_LINUXGNU_ROOT']}")
     raise "No such file or directory - "
   end
rescue Exception => e
  puts e.message + "Please, set env variable 'MIPS_LINUXGNU_ROOT'"
end

path_to_mipsgcc = "#{ENV['MIPS_LINUXGNU_ROOT']}" + "/bin/mips-mti-linux-gnu-gcc "

cmd_line = String.new

case ARGV[0]
when 'openwrt'
  cmd_line = path_to_mipsgcc + "-static -march=24kc -o #{executable} #{src_dir}"
when 'wiveng'
  cmd_line = path_to_mipsgcc + "-static -EL -o #{executable} #{src_dir}"
end

%x[#{cmd_line}]
