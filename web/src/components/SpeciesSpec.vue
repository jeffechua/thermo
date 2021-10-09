<template>
  <div class="item-box">
    <div class="contents">
      <table>
        <field
          label="name"
          type="text"
          v-model="element_.name"
          :running="running"
        />
        <field
          label="molar mass (kg/mol)"
          type="positive number"
          v-model="element_.molarMass"
          :running="running"
        />
        <field
          label="chem. enthalpy (J/mol)"
          type="number"
          v-model="element_.chemH_"
          :running="running"
        />
      </table>
    </div>
    <list-nav
      v-if="!running"
      :first="first"
      :last="last"
      @moveup="$emit('moveup')"
      @delete="$emit('delete')"
      @movedown="$emit('movedown')"
    />
  </div>
</template>

<script>
import ListNav from "./ListNav.vue";
import Field from "./Field.vue";

export default {
  name: "SpeciesSpec",
  props: ["element", "running", "first", "last"],
  emits: ["moveup", "delete", "movedown"],
  components: { ListNav, Field },
  data() {
    return {
      // evilly bypass the prop mutation restriction
      element_: this.element,
    };
  },
};
</script>