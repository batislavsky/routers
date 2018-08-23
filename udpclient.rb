require 'socket'
require 'timeout'

def ip_to_long(ip)
  ip = ip.split(".").map do |x|
    x.to_i
  end
  ip[3] + (ip[2] * 2**8) + (ip[1] * 2**16) + (ip[0] * 2**24)
end

def cur_time
  time = Time.new
  "%02d:%02d:%02d" % [time.hour, time.min, time.sec]
end

class Datagram
  attr_accessor :dg_arr
  def initialize(hostadr, message)
    @dg_arr = {:bindport => [hostadr[:port]].pack("S"),
               :hostip => [hostadr[:ip]].pack("L"),
               :msg => message}
  end

  def senddg(s, ip, port)
    make_datagram
    s.send @datagram, 0, ip, port.to_i
    puts "|#{cur_time}| SEND: message = #{@dg_arr[:msg]}; to = #{ip}:#{port}"
  end

  private
  def make_datagram
    @datagram = String.new
    @datagram << dg_arr[:hostip] << dg_arr[:bindport] << dg_arr[:msg]
  end

end

begin

  raise "Specify port for bind()" if ARGV[0].nil?
  raise "Specify ip:port for send()" if ARGV[1].nil?
  sock = UDPSocket.new
  sock.bind(Socket::INADDR_ANY, ARGV[0].to_i)

rescue Exception => e
  puts e.message
  exit(0)
end

router_addr = ARGV[1].split(':')

hostip = nil
begin

hostip = %x[hostname -I].strip.split(" ")[0]
myadr = {:ip => ip_to_long(hostip),
         :port => ARGV[0].to_i}

rescue Exception=>e

puts e.message
hostip = "192.168.1.92"
myadr = {:ip => ip_to_long(hostip),
         :port => ARGV[0].to_i}
end

puts "hostaddr: #{hostip}:#{ARGV[0]}"

msg = nil
until msg == "stop"

  print "msg: "; msg = STDIN.gets.strip
  dg = Datagram.new(myadr, msg)

  begin
    dg.senddg sock, router_addr[0], router_addr[1].to_i
    clock_start= Process.clock_gettime(Process::CLOCK_REALTIME, :float_millisecond)

    if msg == "stop"
      break
    end

    Timeout.timeout(10) do
      rcvmsg, addrinfo = sock.recvfrom(1024)
      clock_end= Process.clock_gettime(Process::CLOCK_REALTIME, :float_millisecond)
      puts "|#{cur_time}| RECIEVE: msg = #{rcvmsg}; from = #{addrinfo[3]}:#{addrinfo[1]}"
      puts "DIF_TIME = " + (clock_end - clock_start).to_s + " milliseconds."
    end

  rescue Exception => e
    puts "|#{cur_time}| " + e.message.upcase
    next
  end

end

sock.close
