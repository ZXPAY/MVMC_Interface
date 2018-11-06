

def getGWB(vin, vout, hz):
	return (vout)/(vin)*hz

	
	
print(getGWB(0.06, 0.95, 100*1000))

print(getGWB(0.06, 0.56, 200*1000))

print(getGWB(0.06, 0.22, 500*1000))

print(getGWB(0.06, 0.13, 1000*1000))
