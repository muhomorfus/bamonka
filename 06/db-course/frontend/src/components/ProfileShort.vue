<template>
  <div class="card mb-2">
    <div class="card-body">
      <h5 v-if="profile.working" class="card-title">{{ profile.name }}</h5>
      <h5 v-else class="card-title text-muted fst-italic">{{ profile.name }} (inactive)</h5>

      <h6 class="card-subtitle text-muted mb-2">@{{ profile.nickname }}</h6>

      <a :href="`/employee/${profile.nickname}`" class="card-link">Go to profile</a>
    </div>
  </div>
</template>

<style scoped>

</style>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions} from "vuex";

export default defineComponent({
  props: ['profile'],
})
</script>
