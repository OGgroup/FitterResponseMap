#!/usr/bin/env python3
import numpy as np

def main():
    outputfile  = 'example.csv'
    ## Example constants
    ## 50 cm vertex resolution
    vertex_resolution = 50
    angular_resolution = 1.5
    ## 100 hits / MeV, with 5% resolution
    nhitspermev = 100
    resolution  = 1.05
    # Loop over energy, px, py, pz
    energy = np.linspace(0,10,6)
    rho    = np.linspace(0, 10000, 5)
    absz   = np.linspace(0, 10000, 5)
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
    px     = []
    py     = []
    pz     = []
    dtheta = []
    emean  = []
    eres   = []
    for e in energy:
        for r in rho:
            for z in absz :
                energy_data.append(e)
                rho_data.append(r)
                z_data.append(z)
                px.append(vertex_resolution)
                py.append(vertex_resolution)
                pz.append(vertex_resolution)
                dtheta.append(angular_resolution)
                emean.append( nhitspermev * e )
                eres.append( resolution*(nhitspermev*e)**0.5 )

    data = np.array([ energy_data, rho_data, z_data,
                      px, py, pz, dtheta, emean, eres ], dtype='object').T
    header = 'energy,rho,z,px,py,pz,dtheta,emean,eres'

    np.savetxt(outputfile, data, delimiter=',', fmt='%s', comments='', header=header)

if __name__ == '__main__':
    main()
