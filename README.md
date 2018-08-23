# routers
August, 2018

owrt_server.c - server on OpenWRT. Running: ./owrt_server <bindport>
wng_server.c - server on WiveNG. Running: ./wng_server <bindport> <next_router_in_chain_ip:next_router_in_chain_port>
sender.rb - compile and send binary file wng_server or owrt_server to router by scp
udpclient.rb - UDP-client. Running: ruby udpclient.rb <bindport> <next_router_in_chain_ip:next_router_in_chain_port>
mk.rb - building owrt_server.c and wng_server.c for Mips. Runnings: ruby mk.rb <openwrt/wiveng> <filename>
  
Toolchain for MIPS:
https://www.mips.com/develop/tools/codescape-mips-sdk/download-codescape-mips-sdk-essentials/.
