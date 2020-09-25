import numpy as np

def main():
    outputfile  = 'example.csv'
    # Loop over energy, px, py, pz
    energy = np.linspace(0,10,6)
    rho    = np.linspace(0, 8000, 5)
    absz   = np.linspace(0, 8000, 5)
    # switch to bin centers
    energy = ((energy+np.roll(energy,1))/2.0)[1:]
    rho    = ((rho+np.roll(rho,1))/2.0)[1:]
    absz   = ((absz+np.roll(absz,1))/2.0)[1:]
    ## columns
    ## Observables
    energy_data = []
    rho_data    = []
    z_data      = []
    ## Parameters
    energy_resn = []
    px          = []
    py          = []
    pz          = []
    ##
    for e in energy:
        for r in rho:
            for z in absz :
                energy_data.append(e)
                rho_data.append(r)
                z_data.append(z)
                energy_resn.append( 1.10*e**0.5 )
                px.append(30)
                py.append(30)
                pz.append(30)

    data = np.array([ energy_data, rho_data, z_data,
                      energy_resn, px, py, pz ], dtype='object').T
    header = 'energy,rho,z,energy_resn,px,py,pz'

    np.savetxt(outputfile, data, delimiter=',', fmt='%s', comments='', header=header)

if __name__ == '__main__':
    main()
