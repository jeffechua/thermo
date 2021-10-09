<template>
  <div class="nav-container">
    <a v-if="!first" class="text-button" @click="$emit('moveup')">△</a>
    <a v-else class="text-button-disabled">△</a>
    <a class="text-button" @click="$emit('delete')">✕</a>
    <a v-if="!last" class="text-button" @click="$emit('movedown')">▽</a>
    <a v-else class="text-button-disabled">▽</a>
  </div>
</template>

<script>
export default {
  name: "ListNav",
  props: ["first", "last"],
  emits: ["moveup", "delete", "movedown"],
};
</script>

<style scoped>
.nav-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  padding-top: 5px;
  padding-bottom: 5px;
}
</style>