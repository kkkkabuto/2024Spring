# 题目
- ctfshow 飞鸽传书
```
TVdJd09HRm1NamMyWkdKak56VTVNekkzTVdZMFpXVTJNVFl5T0Rrek1qUWxNRUZsTW1GbE0yRXlNelV3TnpRell6VXhObU5rWVRReE1qUTVPV0poTTJKbE9TVXdRV0prWlRVeVkySXpNV1JsTXpObE5EWXlORFZsTURWbVltUmlaRFptWWpJMEpUQkJaVEl6WlRBd1ltVXpPV1F6Tm1Zek5EWXlaVFUzTm1FMk4yRTNaamt4T1RrbE1FRXhPR00zT1RJNE5XSTFNVFJqTmpObVl6a3dNelZsTTJZNU1qQmhaVFEzTnlVd1FXUmhORFJrWkRFNU1tUmxabVF4WW1VM09XWTJNMk16TlRCa01qa3lNR05tSlRCQk5ESTFNV00wWXpZME9XTTNaREptT0RZek1qZGxabVJsTWpNNU9USm1ZVGNsTUVGaFlXVTNZakprTkRneU16Z3lZV0ZoWkRjMVptUmxOalJrWmpobVpqZzJaaVV3UVRJNU5tWTNabVpqTW1VME5UUTFaR00zTnpreU1EVXdZMlZpTkdFNE56RXhKVEJCTmpFd04yRmpNV0UxTldZeFpUQm1aV05pTjJSa1lqWXdabUl6WW1ZeE1Ea2xNRUZoWldNeU16TXpNekl4WkRjek1EQXdNVFl4TmpneVpETmpOR1ZpWXpBd09TVXdRVFV3TURWaU0ySm1NREF3TlRCaVpqUm1OMlUwTTJGak16TmhNRFExTkdJNEpUQkI=
```

# 解题
一眼base64，解码
```
MWIwOGFmMjc2ZGJjNzU5MzI3MWY0ZWU2MTYyODkzMjQlMEFlMmFlM2EyMzUwNzQzYzUxNmNkYTQxMjQ5OWJhM2JlOSUwQWJkZTUyY2IzMWRlMzNlNDYyNDVlMDVmYmRiZDZmYjI0JTBBZTIzZTAwYmUzOWQzNmYzNDYyZTU3NmE2N2E3ZjkxOTklMEExOGM3OTI4NWI1MTRjNjNmYzkwMzVlM2Y5MjBhZTQ3NyUwQWRhNDRkZDE5MmRlZmQxYmU3OWY2M2MzNTBkMjkyMGNmJTBBNDI1MWM0YzY0OWM3ZDJmODYzMjdlZmRlMjM5OTJmYTclMEFhYWU3YjJkNDgyMzgyYWFhZDc1ZmRlNjRkZjhmZjg2ZiUwQTI5NmY3ZmZjMmU0NTQ1ZGM3NzkyMDUwY2ViNGE4NzExJTBBNjEwN2FjMWE1NWYxZTBmZWNiN2RkYjYwZmIzYmYxMDklMEFhZWMyMzMzMzIxZDczMDAwMTYxNjgyZDNjNGViYzAwOSUwQTUwMDViM2JmMDAwNTBiZjRmN2U0M2FjMzNhMDQ1NGI4JTBB
```

再次base64解码
```
1b08af276dbc7593271f4ee616289324%0Ae2ae3a2350743c516cda412499ba3be9%0Abde52cb31de33e46245e05fbdbd6fb24%0Ae23e00be39d36f3462e576a67a7f9199%0A18c79285b514c63fc9035e3f920ae477%0Ada44dd192defd1be79f63c350d2920cf%0A4251c4c649c7d2f86327efde23992fa7%0Aaae7b2d482382aaad75fde64df8ff86f%0A296f7ffc2e4545dc7792050ceb4a8711%0A6107ac1a55f1e0fecb7ddb60fb3bf109%0Aaec2333321d73000161682d3c4ebc009%0A5005b3bf00050bf4f7e43ac33a0454b8%0A
```

url解码
```
1b08af276dbc7593271f4ee616289324
e2ae3a2350743c516cda412499ba3be9
bde52cb31de33e46245e05fbdbd6fb24
e23e00be39d36f3462e576a67a7f9199
18c79285b514c63fc9035e3f920ae477
da44dd192defd1be79f63c350d2920cf
4251c4c649c7d2f86327efde23992fa7
aae7b2d482382aaad75fde64df8ff86f
296f7ffc2e4545dc7792050ceb4a8711
6107ac1a55f1e0fecb7ddb60fb3bf109
aec2333321d73000161682d3c4ebc009
5005b3bf00050bf4f7e43ac33a0454b8
```

32位，md5解密，每一个都解一下，连起来就是flag
http://www.ttmd5.com/

# flag
```
flag{36D_me}
```
