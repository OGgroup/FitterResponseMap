import numpy as np

def main():
    outputfile  = 'example.csv'
    # Loop over energy, px, py, pz
    energy = np.linspace(0,10,11)
    rho    = np.linspace(0, 8000, 9)
    absz   = np.linspace(0, 8000, 9)
    ## columns
    ## Observables
    target      = []
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
                target.append('wbls_3pct')
                energy_data.append(e)
                rho_data.append(r)
                z_data.append(z)
                energy_resn.append( 1.10*e**0.5 )
                px.append(30)
                py.append(30)
                pz.append(30)

    data = np.array([ target, energy_data, rho_data, z_data,
                      energy_resn, px, py, pz ], dtype='object').T
    header = 'target,energy,rho,z,energy_resn,px,py,pz'

    np.savetxt(outputfile, data, delimiter=',', fmt='%s', comments='', header=header)

if __name__ == '__main__':
    main()
