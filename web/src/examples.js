export default {
    "timestep": 0.01,
    "species": [
        {
            "name": "oxygen",
            "molarMass": 0.032,
            "chemH_": 0
        },
        {
            "name": "nitrogen",
            "molarMass": 0.028,
            "chemH_": 0
        },
        {
            "name": "oxygen",
            "molarMass": 0.032,
            "chemH_": 0
        },
        {
            "name": "nitrogen",
            "molarMass": 0.028,
            "chemH_": 0
        }
    ],
    "bases": [
        {
            "name": "air",
            "gamma": 1.4,
            "components": [
                "oxygen",
                "nitrogen"
            ]
        }
    ],
    "nodes": [
        {
            "type": "SourceSink",
            "name": "nitrogen main",
            "basis": "air",
            "P": 1.5e5,
            "T": 298,
            "xs": [
                0.98,
                0.02
            ]
        },
        {
            "type": "IsochoricCell",
            "name": "cylinder",
            "basis": "air",
            "P": 1.1e5,
            "V": 0.5,
            "T": 298,
            "xs": [
                0.79,
                0.21
            ]
        },
        {
            "type": "IsobaricCell",
            "name": "tank",
            "basis": "air",
            "P": 1e5,
            "T": 298,
            "ns": [
                0,
                0
            ]
        }
    ],
    "edges": [
        {
            "type": "VelocityHeadLossPipe",
            "name": "line to cylinder",
            "origin": "nitrogen main",
            "target": "cylinder",
            "diameter": 0.1,
            "headsLost": 1
        },
        {
            "type": "VelocityHeadLossPipe",
            "name": "line to tank",
            "origin": "cylinder",
            "target": "tank",
            "diameter": 0.1,
            "headsLost": 1
        }
    ]
};