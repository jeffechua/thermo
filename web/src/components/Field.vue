<template>
  <tr>
    <th>{{ label }}:</th>
    <td v-if="running">{{ field }}</td>
    <td v-else><input ref="input" v-model.lazy="field" /></td>
  </tr>
</template>

<script>
export default {
  name: "Field",
  props: ["label", "modelValue", "type", "running"],
  emits: ["update:modelValue"],
  computed: {
    field: {
      get() {
        return this.modelValue;
      },
      set(newValue) {
        if (this.type == "text") {
          this.$emit("update:modelValue", newValue);
        } else {
          let parsed = parseFloat(newValue);
          if (this.type == "positive number") parsed = parsed < 0 ? 0 : parsed;
          if (isNaN(parsed)) {
            this.$refs.input.value = this.modelValue;
          } else {
            this.$refs.input.value = parsed;
            this.$emit("update:modelValue", parsed);
          }
        }
      },
    },
  },
};
</script>