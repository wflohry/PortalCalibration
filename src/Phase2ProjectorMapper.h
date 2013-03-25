#ifndef _PORTAL_CALIBRATION_PHASE_2_PROJECTOR_MAPPER_H_
#define _PORTAL_CALIBRATION_PHASE_2_PROJECTOR_MAPPER_H_

/**
 * This class maps phase values to projector points (pixels on its DMD)
 *
 * This class is currently using the 5 step, multiwavelength method, but should
 * be generic to take any method for future expansion (current TODO).
 */
class Phase2ProjectorMapper
{
    private:
        const float m_phi0;
        const float m_phasePitch;

    public:
        Phase2ProjectorMapper(float phi0, float phasePitch);
        float Map2ProjectorCoordinate(float phase);
}

#endif //_PORTAL_CALIBRATION_PHASE_2_PROJECTOR_MAPPER_H_
