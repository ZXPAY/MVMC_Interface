
Eo_0 = 0.3755
Eo_1 = 0.3790

delta_offset = (Eo_1 - Eo_0)/1001


delta_com0 = 0.0003
delta_com1 = 0.0004
delta_com = delta_com1 - delta_com0

CMRR = 1001 * (delta_com/delta_offset)

print('delta_com : ', delta_com)
print('delta_offset : ', delta_offset)
print('CMRR : 	', CMRR)







