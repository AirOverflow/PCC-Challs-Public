# OverAirFlow

---

## Challenge Description


**Author:** MikiVirus

## Solution


Looking at OBEX protocol which transfers binary data between devices

![File-Output](./images/iamge-1.png)



lets extract the data from the packet
![file-output](./images/image-2.png)

using Tshark we can extract all the binary data 
```bash
tshark -r .\Over-Air-Flow.pcapng -T fields -e obex.header.value.byte_sequence -Y "obex"
```

![file-output](./images/image-3.png)

remove all the new lines and go to [CyberChef](https://gchq.github.io/CyberChef/) and use _From Hex_ and save the output 
![file-output](./images/image-4.png)

FLAG:
![file-output](./images/image-5.png)


---




