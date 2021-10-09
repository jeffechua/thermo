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
          label="gamma"
          type="positive number"
          v-model="element_.gamma"
          :running="running"
        />
        <tr>
          <th>components:</th>
          <td style="display: flex; flex-direction: column">
            <select @change="addComponent" v-if="!running">
              <option value="-1">- select -</option>
              <option
                v-for="species of data.filter(
                  (s) => !element_.components.includes(s.id)
                )"
                :key="species.id"
                :value="species.id"
              >
                {{ species.name }}
              </option>
            </select>
            <span v-for="id of element_.components" :key="id">
              <button v-if="!running" @click="removeComponent(id)">
                ✕ {{ derefID(id, data).name }}
              </button>
              <span v-else>{{ derefID(id, data).name }}</span>
            </span>
          </td>
        </tr>
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
import { derefID } from "../utils.js";

export default {
  name: "BasisSpec",
  props: ["element", "running", "first", "last", "data"],
  emits: ["moveup", "delete", "movedown"],
  components: { ListNav, Field },
  data() {
    return {
      // evilly bypass the prop mutation restriction
      element_: this.element,
    };
  },
  methods: {
    derefID: derefID,
    addComponent(event) {
      let id = parseInt(event.target.value);
      event.target.value = -1;
      if (id == "-1") return;
      if (this.element_.components.includes(id)) return;
      this.element_.components.push(id);
    },
    removeComponent(id) {
      let index = this.element_.components.indexOf(id);
      if (index != -1) this.element_.components.splice(index, 1);
    },
  },
};
</script>