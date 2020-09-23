import numpy as np

def main():
    outputfile  = 'example.csv'
    energy      = np.linspace(0,10,11)
    energy_resn = 1.10*energy**0.5
    target      = np.array(['wbls_3pct' for x in energy])

    header = 'target,energy,energy_resn'
    data = np.array([ target, energy, energy_resn ]).T

    np.savetxt(outputfile, data, delimiter=',', fmt='%s', comments='', header=header)

if __name__ == '__main__':
    main()
