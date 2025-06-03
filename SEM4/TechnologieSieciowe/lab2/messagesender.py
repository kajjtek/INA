from scapy.all import *


laptop1_ip = "172.20.10.7"  
laptop2_ip = "172.20.10.6"  
laptop2_port = 12345

# Create the IP packet with the spoofed source IP (laptop3's IP) and destination IP (laptop1's IP)
ip_packet = IP(src=laptop1_ip, dst=laptop2_ip)

# Create the UDP packet
# You can put your custom message as the payload in the Raw() layer
custom_message = ""

udp_packet = UDP(sport=RandShort(), dport=laptop2_port) / Raw(load=custom_message)

# Combine the IP and UDP packets
packet = ip_packet / udp_packet

# Send the crafted packet
send(packet)