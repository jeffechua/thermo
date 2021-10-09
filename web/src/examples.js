export default {
    "nextID": 8,
    "timestep": 0.01,
    "species": [
        {
            "id": 0,
            "name": "oxygen",
            "molarMass": 0.032,
            "chemH_": 0
        },
        {
            "id": 1,
            "name": "nitrogen",
            "molarMass": 0.028,
            "chemH_": 0
        }
    ],
    "bases": [
        {
            "id": 2,
            "name": "air",
            "gamma": 1.4,
            "components": [0, 1]
        }
    ],
    "nodes": [
        {
            "id": 3,
            "type": "SourceSink",
            "format": "PTx",
            "name": "nitrogen main",
            "basis": "2",
            "P": 1.5e5,
            "V": 0,
            "T": 298,
            "H": 0,
            "xs": [0.98, 0.02],
            "ns": []
        },
        {
            "id": 4,
            "type": "IsochoricCell",
            "format": "PVTx",
            "name": "cylinder",
            "basis": "2",
            "P": 1.1e5,
            "V": 0.5,
            "T": 298,
            "H": 0,
            "xs": [0.79, 0.21],
            "ns": []
        },
        {
            "id": 5,
            "type": "IsobaricCell",
            "format": "PTn",
            "name": "tank",
            "basis": "2",
            "P": 1e5,
            "V": 0,
            "T": 298,
            "H": 0,
            "xs": [1, 0],
            "ns": [0, 0]
        }
    ],
    "edges": [
        {
            "id": 6,
            "type": "VelocityHeadLossPipe",
            "name": "line to cylinder",
            "origin": 3,
            "target": 4,
            "diameter": 0.1,
            "headsLost": 1
        },
        {
            "id": 7,
            "type": "VelocityHeadLossPipe",
            "name": "line to tank",
            "origin": 4,
            "target": 5,
            "diameter": 0.1,
            "headsLost": 1
        }
    ]
};