# Example of work

## without CIDR

```
Input ip address: 192.168.1.1
In BIN(2): 11000000.10101000.00000001.00000001
In HEX(16): c0.a8.1.1
In OCT(8): 300.250.1.1
CIDR y/n : n
!Common address!
C class:
Net ID = 11000000.10101000.00000001
Host ID = 00000001
```

## with CIDR

```
Input ip address: 192.168.1.1
In BIN(2): 11000000.10101000.00000001.00000001
In HEX(16): c0.a8.1.1
In OCT(8): 300.250.1.1
CIDR y/n : y
Input CIDR: /19

Net ID = 11000000.10101000.000
Host ID = 00001.00000001
```
