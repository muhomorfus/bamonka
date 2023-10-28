<template>
  <nav aria-label="breadcrumb">
    <ol class="breadcrumb">
      <li class="breadcrumb-item" v-for="bc in breadcrumbs" :key="bc.id"><a :href="`/departments/${bc.id}`">{{ bc.name }}</a></li>
      <li class="breadcrumb-item active" aria-current="page">{{ last }}</li>
    </ol>
  </nav>
</template>

<style scoped>

</style>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions} from "vuex";

export default defineComponent({
  props: ['breadcrumbs', 'last'],
})
</script>
