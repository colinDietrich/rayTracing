# Introduction

This report aims to develop a ray-tracing software for calculating the propagation of electromagnetic waves. The software can compute the power captured by a device connected to a Wifi access point within a building. It can also determine the coverage area of a base station of an indoor small cell of a 5G network and the bit rate received by a receiver relative to its position.

## Assumptions

To simplify analytical developments and optimize the digital simulation program, various assumptions will be made in the project:
1. **Assumptions about antennas:**
    - Several assumptions are made for the antennas:
        - It is considered a 5G network operating at a frequency of 27 GHz with transmitters and receivers corresponding to half-wavelength dipole antennas without losses and oriented along the z-axis.
        - It is assumed that the transmission power of the antennas is 20 dBm.
        - It is admitted that transmitters and receivers are all located at the same height from the ground.
        - Only the plane waves radiated by the antennas propagating in the horizontal plane (x, y) are studied.
    
2. **Far-field approximation:**
    - It is assumed that the far-field approximation can be applied to the entire building in which the coverage area is studied. The boundary between the near-field and far-field regions is conventionally defined at a distance \(r_{ff}\) from the transmitter such that:
      \[ r_{ff} = \textbf{Max} \{1.6\lambda ; 5D ; \frac{2D^2}{\lambda}\} = 5D = 0.0278m \]
      with \(D = \frac{\lambda}{2} = 0.00556m\) the maximum characteristic dimension of the transmitter.
    - This assumption is therefore valid as long as the receiver is at a distance of more than 2.78 cm from the transmitter, which will always be verified.
    
3. **Plane waves:**
    - The waves are assumed to be locally plane. Indeed, as seen in the course, each interaction of a wave with the environment can be studied by assuming that the wave is locally plane, provided that the distance from the sources is sufficient, which is indeed verified with the far-field assumption.
    
4. **Interactions with the environment:**
    - The interactions of plane waves with the environment are calculated using the models seen in the course. Thus, only waves that have undergone one, two, or three reflections and have been transmitted through walls are taken into account. Diffraction is neglected.
    - Finally, furniture, doors, and people inside the building are also not taken into account.

# Software

The software was developed in C++ because the object-oriented programming paradigm allows for easier program modeling, and the computational power provided by this language allows for optimal execution times.

**Modes of operation**
The software implements ray-tracing and has three different operating modes:
- **Mode 1**: It allows displaying all the rays that will reach the receiver for one or more transmitters and the calculation of the power received by it. The user can choose the position of the transmitters and the receiver.
- **Mode 2**: It allows visualizing the coverage area and its intensity for one or more transmitters. The coverage area is calculated by discretizing the map. Indeed, the software calculates the power received by a receiver placed in each of the elements of the discretization. The user chooses the position of the transmitters and the map discretization value, which greatly influences the calculation time.
- **Mode 3**: It determines the number of transmitters, as well as their positions, ensuring optimal coverage for a given map. This determination is made using a genetic algorithm whose operation is explained in the optimization section.

# Optimization

The goal of this section is to determine the number and positioning of transmitters so that the 5G network optimally covers the museum floor with as few transmitters as possible.
Thus, to solve this problem with a reasonable execution time, we use a genetic algorithm inspired by the chromosome replication method. Indeed, it does not require determining all possible cases for the positioning of the sources (which would take a very long calculation time). Moreover, it allows not to have to first fix the number of sources and then determine their optimal positioning. The algorithm, theory, and images presented in this section are greatly inspired by the article _"Wireless Heterogeneous Transmitter Placement Using Multiobjective Variable-Length Genetic Algorithm"_ by C. Ting, C. Lee, H. Chang, and J. Wu in IEEE Transactions on Systems, Man, and Cybernetics, Part B (Cybernetics), vol. 39, no. 4, pp. 945-958, Aug. 2009.
