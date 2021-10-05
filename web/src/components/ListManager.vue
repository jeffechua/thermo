<template>
  <div>
    <span>species</span>
    <a
      v-if="!running"
      class="text-button"
      @click="list_.push({ id: indexer_.nextID++ })"
      >+</a
    >
    <div v-for="(item, i) of list_" :key="item.id">
      <component
        v-bind:is="component"
        v-bind:element="list_[i]"
        v-bind:running="running"
        v-bind:first="i == 0"
        v-bind:last="i == list_.length - 1"
        @moveup="list_.splice(i - 1, 0, list_.splice(i, 1)[0])"
        @delete="list_.splice(i, 1)"
        @movedown="list_.splice(i + 1, 0, list_.splice(i, 1)[0])"
      />
    </div>
  </div>
</template>

<script>
export default {
  name: "SpeciesSpec",
  props: ["indexer", "list", "component", "running"],
  data() {
    return {
      // evilly bypass the prop mutation restriction
      list_: this.list,
      indexer_: this.indexer,
    };
  },
};
</script>