<template>
  <div class="#app">
    <p>timestep: {{ spec.timestep }}</p>
    <p>species:</p>
    <div style="width: 100%; display: flex">
      <div>
        <div v-for="(item, i) in spec.species" :key="i">
          <species v-model="spec.species[i]" v-bind:locked="true" />
        </div>
      </div>
      <div>
        <div v-for="(item, i) in spec.species" :key="i">
          <species v-model="spec.species[i]" v-bind:locked="false" />
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import species from "./components/species.vue";
import example from "./examples.js";

export default {
  name: "App",
  components: {
    species,
  },
  data() {
    console.log(example);
    return { spec: example };
  },
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
