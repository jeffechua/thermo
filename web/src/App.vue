<template>
  <div class="#app">
    <p>timestep: {{ spec.timestep }}</p>
    <div style="display: flex">
      <div class="flex-grow">paper</div>
      <div>
        <span>species</span>
        <a class="text-button" @click="spec.species.push({ id: spec.nextID++ })"
          >+</a
        >
        <div v-for="(item, i) of spec.species" :key="item.id">
          <species-spec
            v-model="spec.species[i]"
            v-bind:running="false"
            v-bind:first="i == 0"
            v-bind:last="i == spec.species.length - 1"
            @moveup="move(spec.species, i - 1, i)"
            @delete="remove(spec.species, i)"
            @movedown="move(spec.species, i + 1, i)"
          />
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import SpeciesSpec from "./components/SpeciesSpec.vue";
import example from "./examples.js";

export default {
  name: "App",
  components: {
    SpeciesSpec,
  },
  data() {
    var spec = example;
    spec.nextID = 0;
    for (var species of spec.species) {
      species.id = spec.nextID;
      spec.nextID++;
    }
    for (var basis of spec.bases) {
      basis.id = spec.nextID;
      spec.nextID++;
    }
    for (var node of spec.nodes) {
      node.id = spec.nextID;
      spec.nextID++;
    }
    for (var edge of spec.edges) {
      edge.id = spec.nextID;
      spec.nextID++;
    }
    return { spec: spec };
  },
  methods: {
    move(array, from, to) {
      array.splice(to, 0, array.splice(from, 1)[0]);
    },
    remove(array, at) {
      array.splice(at, 1);
    },
  },
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: #2c3e50;
  margin-top: 60px;
}
.item-box {
  border: 1px solid black;
  border-radius: 5px;
  display: flex;
}
.item-box .contents {
  flex-grow: 1;
  flex-shrink: 0;
  padding-left: 5px;
  padding-right: 5px;
}
.item-box th {
  text-align: right;
  padding-right: 3px;
}
.flex-grow {
  flex-grow: 1;
  flex-shrink: 0;
}
.text-button {
  color: #808080;
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
