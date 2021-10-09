<template>
  <div class="nav-container">
    <a class="text-button" :class="{ disabled: first }" @click="$emit('moveup')"
      >△</a
    >
    <a class="text-button" @click="$emit('delete')">✕</a>
    <a
      class="text-button"
      :class="{ disabled: last }"
      @click="$emit('movedown')"
      >▽</a
    >
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