import FWCore.ParameterSet.Config as cms

_barrelAlgo = cms.PSet(
    algoName = cms.string("MTDRecHitAlgo"),
    thresholdToKeep = cms.double(1.), # [MeV]
    calibrationConstant = cms.double(1.)
)


_endcapAlgo = cms.PSet(
    algoName = cms.string("MTDRecHitAlgo"),
    thresholdToKeep = cms.double(0.0425),    # MeV
    calibrationConstant = cms.double(0.085), # MeV/MIP
)

from Configuration.Eras.Modifier_phase2_etlV4_cff import phase2_etlV4
phase2_etlV4.toModify(_endcapAlgo, thresholdToKeep = 0.005, calibrationConstant = 0.015 )

mtdRecHits = cms.EDProducer(
    "MTDRecHitProducer",
    barrel = _barrelAlgo,
    endcap = _endcapAlgo,
    barrelUncalibratedRecHits = cms.InputTag('mtdUncalibratedRecHits:FTLBarrel'),
    endcapUncalibratedRecHits = cms.InputTag('mtdUncalibratedRecHits:FTLEndcap'),
    BarrelHitsName = cms.string('FTLBarrel'),
    EndcapHitsName = cms.string('FTLEndcap'),
)

from SimFastTiming.FastTimingCommon.mtdDigitizer_cfi import mtdDigitizer
mtdRecHitsSoA = cms.EDProducer('btlrechit::BTLRecHitSoAProducer@alpaka',
    uncalibrh = cms.InputTag("mtdUncalibratedRecHitsSoA"),
    adcNbits = mtdDigitizer.barrelDigitizer.ElectronicsSimulation.adcNbits,
    adcSaturation = mtdDigitizer.barrelDigitizer.ElectronicsSimulation.adcSaturation_MIP,
    toaLSB_ns = mtdDigitizer.barrelDigitizer.ElectronicsSimulation.toaLSB_ns,
    timeCorr_p0 = cms.double( 2.21103),
    timeCorr_p1 = cms.double(-0.933552),
    timeCorr_p2 = cms.double( 0.),
    c_LYSO = cms.double(13.846235)     # in unit cm/ns
    )



