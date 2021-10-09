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
          :running="false"
        />
        <list-manager
          v-if="tab == 'bases'"
          :list="spec.bases"
          :component="basisSpec"
          :running="false"
          :data="spec.species"
        />
      </div>
    </div>
  </div>
</template>

<script>
import ListManager from "./components/ListManager.vue";
import SpeciesSpec from "./components/SpeciesSpec.vue";
import BasisSpec from "./components/BasisSpec.vue";
import example from "./examples.js";

export default {
  name: "App",
  components: {
    ListManager,
  },
  data() {
    return { spec: example, tab: "species" };
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
  border-spacing: 0px;
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
  font-weight: inherit;
  font-size: inherit;
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
.text-button-disabled {
  color: #e0e0e0;
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
</style>
