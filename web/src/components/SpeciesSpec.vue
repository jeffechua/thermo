<template>
  <div class="item-box">
    <div class="contents">
      <table>
        <tr>
          <th>name:</th>
          <td v-if="running">{{ species.name }}</td>
          <td v-else>
            <input type="text" v-model="species.name" @input="onChange" />
          </td>
        </tr>
        <tr>
          <th>molar mass (kg/mol):</th>
          <td v-if="running">{{ species.molarMass }}</td>
          <td v-else>
            <input
              type="number"
              v-model="species.molarMass"
              @input="onChange"
            />
          </td>
        </tr>
        <tr>
          <th>chem. enthalpy (J/mol):</th>
          <td v-if="running">{{ species.chemH_ }}</td>
          <td v-else>
            <input type="number" v-model="species.chemH_" @input="onChange" />
          </td>
        </tr>
      </table>
    </div>
    <list-nav
      v-if="!running"
      v-bind:first="first"
      v-bind:last="last"
      @moveup="$emit('moveup')"
      @delete="$emit('delete')"
      @movedown="$emit('movedown')"
    />
  </div>
</template>

<script>
import ListNav from "./ListNav.vue";

export default {
  name: "SpeciesSpec",
  props: ["modelValue", "running", "first", "last"],
  emits: ["update:modelValue", "moveup", "delete", "movedown"],
  components: { ListNav },
  data() {
    return {
      species: this.modelValue,
    };
  },
  methods: {
    onChange() {
      this.$emit("update:modelValue", this.species);
    },
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
