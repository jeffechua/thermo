<template>
  <table>
    <tr>
      <td>name</td>
      <td v-if="locked">{{ species.name }}</td>
      <td v-else>
        <input type="text" v-model="species.name" @input="onChange" />
      </td>
    </tr>
    <tr>
      <td>molar mass</td>
      <td v-if="locked">{{ species.molarMass }}</td>
      <td v-else>
        <input
          type="number"
          v-model="species.molarMass"
          @input="onChange"
        />
      </td>
    </tr>
    <tr>
      <td>chemical enthalpy</td>
      <td v-if="locked">{{ species.chemH_ }}</td>
      <td v-else>
        <input
          type="number"
          v-model="species.chemH_"
          @input="onChange"
        />
      </td>
    </tr>
  </table>
</template>

<script>
export default {
  name: "HelloWorld",
  props: {
    modelValue: Object,
    locked: Boolean,
  },
  emits: ["update:modelValue"],
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
