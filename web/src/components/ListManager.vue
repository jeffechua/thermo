<template>
  <div>
    <component
      v-for="(item, i) of list_"
      :key="item.id"
      :is="component"
      :element="list_[i]"
      :running="running"
      :first="i == 0"
      :last="i == list_.length - 1"
      :data="data"
      @moveup="list_.splice(i - 1, 0, list_.splice(i, 1)[0])"
      @delete="list_.splice(i, 1)"
      @movedown="list_.splice(i + 1, 0, list_.splice(i, 1)[0])"
    />
  </div>
</template>

<script>
export default {
  name: "SpeciesSpec",
  props: ["label", "list", "generator", "component", "running", "data"],
  data() {
    return {
      // evilly bypass the prop mutation restriction
      list_: this.list,
      root_: this.root,
    };
  },
};
</script>