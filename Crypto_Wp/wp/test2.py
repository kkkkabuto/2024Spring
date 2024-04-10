import gmpy2
import libnum
from Crypto.Util.number import *
from test import RRSSAA_power

p = 12076532702818803027742169983530419558608401078508017894707093811716696786941308547797368731019670776508448150953432566915232808757060410156378938522359551
q = 9504548564498461029558227822137431209369699669992479992757942960885213061136352518231937836400544570835645335056229054429984730840065504477100420427103027
c = (
    59282499553838316432691001891921033515315025114685250219906437644264440827997741343171803974602058233277848973328180318352570312740262258438252414801098965814698201675567932045635088203459793209871900350581051996552631325720003705220037322374626101824017580528639787490427645328264141848729305880071595656587,
    73124265428189389088435735629069413880514503984706872237658630813049233933431869108871528700933941480506237197225068288941508865436937318043959783326445793394371160903683570431106498362876050111696265332556913459023064169488535543256569591357696914320606694493972510221459754090751751402459947788989410441472,
)
n = 114781991564695173994066362186630636631937111385436035031097837827163753810654819119927257768699803252811579701459939909509965376208806596284108155137341543805767090485822262566517029632602553357332822459669677106313003586646066752317008081277334467604607046796105900932500985260487527851613175058091414460877
e = 4252707129612455400077547671486229156329543843675524140708995426985599183439567733039581012763585270550049944715779511394499964854645012746614177337614886054763964565839336443832983455846528585523462518802555536802594166454429110047032691454297949450587850809687599476122187433573715976066881478401916063473308325095039574489857662732559654949752850057692347414951137978997427228231149724523520273757943185561362572823653225670527032278760106476992815628459809572258318865100521992131874267994581991743530813080493191784465659734969133910502224179264436982151420592321568780882596437396523808702246702229845144256038
phi = (p**2 + p + 1) * (q**2 + q + 1)
d = inverse(e, phi)
m = RRSSAA_power(c, d, n)
flag = b"".join([long_to_bytes(v)[:24] for v in m])
print(flag)
print([long_to_bytes(v)[:24] for v in m])
# flag{4872c7e4cc11508f8325f6fb68512a23}
