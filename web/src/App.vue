<template>
  <div class="#app">
    <div style="display: flex">
      <div class="flex-grow">paper</div>
      <div>
        <div style="display: flex; align-items: center">
          <a class="text-button" @click="add">+</a>
          <div class="flex-grow" />
          <button
            class="tab"
            :class="{ active: tab == 'species' }"
            @click="tab = 'species'"
          >
            Species
          </button>
          <button
            class="tab"
            :class="{ active: tab == 'bases' }"
            @click="tab = 'bases'"
          >
            Bases
          </button>
          <button
            class="tab"
            :class="{ active: tab == 'nodes' }"
            @click="tab = 'nodes'"
          >
            Nodes
          </button>
          <button
            class="tab"
            :class="{ active: tab == 'edges' }"
            @click="tab = 'edges'"
          >
            Edges
          </button>
        </div>
        <hr style="margin-top: 0px" />
        <list-manager
          v-if="tab == 'species'"
          :list="spec.species"
          :component="speciesSpec"
          :running="running"
        />
        <list-manager
          v-if="tab == 'bases'"
          :list="spec.bases"
          :component="basisSpec"
          :running="running"
          :data="spec.species"
        />
        <list-manager
          v-if="tab == 'nodes'"
          :list="spec.nodes"
          :component="nodeSpec"
          :running="running"
          :data="spec"
        />
        <div style="height:50px"/>
        <table>
          <tr>
            <td>timestep (s):</td>
            <td><input type="number" step="0.01" v-model="spec.timestep" /></td>
          </tr>
          <tr>
            <td>running (s):</td>
            <td><input type="checkbox" v-model="running" /></td>
          </tr>
        </table>
      </div>
    </div>
  </div>
</template>

<script>
import ListManager from "./components/ListManager.vue";
import SpeciesSpec from "./components/SpeciesSpec.vue";
import BasisSpec from "./components/BasisSpec.vue";
import NodeSpec from "./components/NodeSpec.vue";
import example from "./examples.js";

export default {
  name: "App",
  components: {
    ListManager,
  },
  data() {
    return { spec: example, tab: "species", running: false };
  },
  computed: {
    speciesSpec: {
      get() {
        return SpeciesSpec;
      },
    },
    basisSpec: {
      get() {
        return BasisSpec;
      },
    },
    nodeSpec: {
      get() {
        return NodeSpec;
      },
    },
  },
  methods: {
    add() {
      switch (this.tab) {
        case "species":
          this.spec.species.push({
            id: this.spec.nextID++,
            name: "",
            molarMass: 0.018,
            chemH_: 0,
          });
          break;
        case "bases":
          this.spec.bases.push({
            id: this.spec.nextID++,
            name: "",
            gamma: 1.4,
            components: [],
          });
          break;
        case "nodes":
          this.spec.nodes.push({
            id: this.spec.nextID++,
            basis: this.spec.bases[0].id,
            name: "",
            type: "SourceSink",
            format: "PTx",
            P: 1e5,
            V: 1,
            T: 298,
            H: 0,
            xs: [1],
            ns: [1],
            gamma: 1.4,
          });
          break;
      }
    },
  },
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
}
table {
  border-collapse: collapse;
}
.tab {
  background-color: inherit;
  float: left;
  border: none;
  outline: none;
  padding: 10px 15px;
  transition: 0.3s;
}
/* Change background color of buttons on hover */
.tab:hover {
  background-color: #ddd;
}
/* Create an active/current tablink class */
.tab.active {
  background-color: #ccc;
}
.item-box {
  border-bottom: 1px solid black;
  padding-top: 2px;
  padding-bottom: 2px;
  display: flex;
}
.item-box .contents {
  flex-grow: 1;
  padding-left: 5px;
  padding-right: 5px;
}
.item-box th {
  text-align: right;
  vertical-align: top;
  padding-right: 3px;
}
.flex-grow {
  flex-grow: 1;
}
.text-button {
  color: #808080;
  font-size: 14px;
  padding-left: 5px;
  padding-right: 5px;
  user-select: none;
  -webkit-user-select: none;
  -khtml-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
}
.text-button:hover {
  color: #101010;
}
.text-button:active {
  color: #505050;
}
.text-button.disabled {
  color: #e0e0e0;
  padding-left: 5px;
  padding-right: 5px;
  user-select: none;
  -webkit-user-select: none;
  -khtml-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
}
</style>
