// -*- Mode: c++ -*-
// Copyright (c) 2003-2004, Daniel Thor Kristjansson
#ifndef SCANSTREAMDATA_H_
#define SCANSTREAMDATA_H_

#include "atscstreamdata.h"
#include "dvbstreamdata.h"
#include "mythtvexp.h"

class MTV_PUBLIC ScanStreamData :
    public virtual MPEGStreamData,
    public ATSCStreamData,
    public DVBStreamData
{
  public:
    explicit ScanStreamData(bool no_default_pid = false);
    ~ScanStreamData() override;

    bool IsRedundant(uint pid, const PSIPTable &psip) const override; // ATSCStreamData
    bool HandleTables(uint pid, const PSIPTable &psip) override; // ATSCStreamData

    using DVBStreamData::Reset;
    void Reset(void) override; // ATSCStreamData

    bool HasEITPIDChanges(const uint_vec_t& /*in_use_pids*/) const override // ATSCStreamData
        { return false; }
    bool GetEITPIDChanges(const uint_vec_t& /*in_use_pids*/,
                          uint_vec_t& /*add_pids*/,
                          uint_vec_t& /*del_pids*/) const override // ATSCStreamData
        { return false; }

    QString GetSIStandard(const QString& guess = "mpeg") const;

    void SetFreesatAdditionalSI(bool freesat_si);

  private:
    bool DeleteCachedTable(const PSIPTable *psip) const override; // ATSCStreamData
    /// listen for additional Freesat service information
    bool m_dvbUkFreesatSi {false};
    bool m_noDefaultPid;
};

inline void ScanStreamData::SetFreesatAdditionalSI(bool freesat_si)
{
    QMutexLocker locker(&m_listenerLock);
    m_dvbUkFreesatSi = freesat_si;
    if (freesat_si)
        AddListeningPID(FREESAT_SI_PID);
    else
        RemoveListeningPID(FREESAT_SI_PID);
}

#endif // SCANSTREAMDATA_H_
